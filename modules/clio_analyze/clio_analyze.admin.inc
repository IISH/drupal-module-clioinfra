<?php

/**
 * Page callback: Form constructor for a form to list and reorder visualizations.
 */
function clio_analyze_admin_overview($form) {
  // Overview of all visualizations, including disabled ones.
  $visualizations = clio_analyze_all_visualizations();
  $form['#tree'] = TRUE;
  $form['help'] = array(
    '#markup' => t("The first item in the list will be the default."),
  );
  foreach ($visualizations as $id => $viz) {
    if ($viz->status == 1) {
      $status = array(
        '#type' => 'checkbox',
        '#default_value' =>  1,
        '#disabled' => TRUE,
      );
    }
    else {
      $status = array('#markup' => '');
    }
    $form['visualizations'][$id]['label'] = array('#markup' => check_plain($viz->label));
    $form['visualizations'][$id]['configure'] = array('#type' => 'link', '#title' => t('configure'), '#href' => 'admin/config/clio/analyze/edit/' . $id);
    $form['visualizations'][$id]['status'] = $status;
    $form['visualizations'][$id]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $viz->label)),
      '#title_display' => 'invisible',
      '#default_value' => $viz->weight,
    );
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save changes'));
  return $form;
}

/**
 * Form submission handler for clio_analyze_admin_overview().
 */
function clio_analyze_admin_overview_submit($form, &$form_state) {
  foreach ($form_state['values']['visualizations'] as $id => $data) {
    if (is_array($data) && isset($data['weight'])) {
      // Only update if this is a form element with weight.
      db_update('clio_analyze_viz')
        ->fields(array('weight' => $data['weight']))
        ->condition('name', $id)
        ->execute();
    }
  }
  // Weights define the order of the visualization tabs.
  menu_rebuild();
  drupal_set_message(t('The visualization ordering has been saved.'));
}

/**
 * Returns HTML for the visualization administration overview form.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_clio_analyze_admin_overview($variables) {
  $form = $variables['form'];

  $rows = array();
  foreach (element_children($form['visualizations']) as $id) {
    $form['visualizations'][$id]['weight']['#attributes']['class'] = array('text-vizualization-order-weight');
    $rows[] = array(
      'data' => array(
        drupal_render($form['visualizations'][$id]['label']),
        drupal_render($form['visualizations'][$id]['weight']),
        drupal_render($form['visualizations'][$id]['configure']),
        drupal_render($form['visualizations'][$id]['status']),
      ),
      'class' => array('draggable'),
    );
  }
  $header = array(t('Label'), t('Weight'), t('Configure'), t('Enabled'));
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'text-vizualization-order')));
  $output .= drupal_render_children($form);

  drupal_add_tabledrag('text-vizualization-order', 'order', 'sibling', 'text-vizualization-order-weight');

  return $output;
}

/**
 * Page callback: Displays the visualization add/edit form.
 *
 * @param object|null $viz
 *   (optional) An object representing a vizualization, with the following properties:
 *   - name: A machine-readable name representing the ID of the visualization
 *     to save. If this corresponds to an existing visualization, that vizualization
 *     will be updated; otherwise, a new vizualization will be created.
 *   - label: The title of the visualization.
 *   - status: (optional) An integer indicating whether the visualization is
 *     enabled (1) or not (0). Defaults to 1.
 *   - weight: (optional) The weight of the visualization, which controls its
 *     placement in visualization lists. If omitted, the weight is set to 0.
 *     Defaults to NULL.
 *   - api: The url of the API providing the visualization.
 *   - height: (optional) The height of the iframe. Defaults to 500 (px).
 *   - parameters: (optional) An associative array of parameters
 *     to include in the query to the API.
 *
 * @return
 *   A form array.
 *
 * @see clio_analyze_menu()
 */
function clio_analyze_admin_viz_page($viz = NULL) {
  if (!isset($viz->label)) {
    drupal_set_title(t('Add visualization'));
    $viz = (object) array(
      'name' => NULL,
      'label' => '',
      'status' => 1,
      'api' => '',
      'height' => 500,
    );
  }
  return drupal_get_form('clio_analyze_admin_viz_form', $viz);
}


/**
 *
 */
function clio_analyze_admin_viz_title($viz) {
  return "Edit visualization " . $viz->label;
}


/**
 * Form constructor for the visualization add/edit form.
 *
 * @param $viz
 *   A vizualization object having the properties:
 *   - name: A machine-readable name representing the ID of the visualization to
 *     save. If this corresponds to an existing visualization, that vizualization will be
 *     updated; otherwise, a new vizualization will be created.
 *   - label: The title of the visualization.
 *   - status: (optional) An integer indicating whether the visualization is
 *     enabled (1) or not (0). Defaults to 1.
 *   - weight: (optional) The weight of the visualization, which controls its
 *     placement in visualization lists. If omitted, the weight is set to 0.
 *   - api: The url of the API providing the visualization.
 *   - height: (optional) The height of the iframe. Defaults to 500 (px).
 *   - parameters: (optional) An associative array of parameters
 *     to include in the query to the API.
 *
 * @see clio_analyze_admin_viz_form_validate()
 * @see clio_analyze_admin_viz_form_submit()
 * @ingroup forms
 */
function clio_analyze_admin_viz_form($form, &$form_state, $viz) {
  $form['#viz'] = $viz;
  $form['#tree'] = TRUE;
  
  $form['help'] = array(
    '#markup' => t("Default parameters for each API are <em>hist</em> ('on' if type is historical), <em>handle</em> (datasetid), <em>ctrlist</em> (comma-separated list of country id's), <em>yearmin</em> and <em>yearmax</em>. Extra parameters, with a fixed value, can be set below."),
  );

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#size' => 20,
    '#maxlength' => 255,
    '#default_value' => $viz->label,
    '#required' => TRUE,
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#required' => TRUE,
    '#size' => 20,
    '#maxlength' => 255,
    '#default_value' => $viz->name,
    '#machine_name' => array(
      'exists' => 'clio_analyze_viz_exists',
    ),
    '#disabled' => !empty($viz->name),
    '#description' => t('A unique machine-readable name. Can only contain lowercase letters, numbers, and underscores.<br>The name will be used in the url.'),
  );
  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => $viz->status,
  );
  $form['api'] = array(
    '#type' => 'textfield',
    '#title' => t('API'),
    '#description' => t('The url of the API providing the visualization.'),
    '#default_value' => $viz->api,
    '#required' => TRUE,
    '#element_validate' => array('clio_validate_api'),
  );
  $form['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#size' => 4,
    '#maxlength' => 4,
    '#field_suffix' => 'px',
    '#description' => t('The height of the iframe.'),
    '#default_value' => $viz->height,
  );

  $form['parameters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Extra parameters'),
    '#description' => t('Extra parameters to include in the query to the API.'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $parameters = !empty($viz->parameters) ? $viz->parameters : array();
  // Add an empty key/value pair to the settings form.
  $parameters[] = array(
    'key' => NULL,
    'value' => NULL,
  );
  $index = 0;
  foreach ($parameters as $param) {
    $form['parameters'][$index++] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      'key' => array(
        '#type' => 'textfield',
        '#title' => t('Key'),
        '#size' => 10,
        '#default_value' => $param['key'],
      ),
      'value' => array(
        '#type' => 'textfield',
        '#title' => t('Value'),
        '#size' => 10,
        '#default_value' => $param['value'],
      ),
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  return $form;
}


/**
 * Form validation handler for clio_analyze_admin_viz_form().
 *
 * @see clio_analyze_admin_viz_form_submit()
 */
function clio_analyze_admin_viz_form_validate($form, &$form_state) {
  $viz_name = trim($form_state['values']['name']);
  $viz_label = trim($form_state['values']['label']);

  if ($form_state['values']['parameters']) {
    foreach ($form_state['values']['parameters'] as $key => $value) {
      if (!$value['key'] || !$value['value']) {
        unset($form_state['values']['parameters'][$key]);
      }
    // TODO else validate (alphanum)?
    }
  }

  // Ensure that the values to be saved later are exactly the ones validated.
  form_set_value($form['name'], $viz_name, $form_state);
  form_set_value($form['label'], $viz_label, $form_state);

  $result = db_query("SELECT name FROM {clio_analyze_viz} WHERE label = :label AND name <> :name", array(':label' => $viz_label, ':name' => $viz_name))->fetchField();
  if ($result) {
    form_set_error('label', t('Visualization labels must be unique. A vizualization labeled %label already exists.', array('%label' => $viz_label)));
  }
}


/**
 * Form submission handler for clio_analyze_admin_viz_form().
 *
 * @see clio_analyze_admin_viz_form_validate()
 */
function clio_analyze_admin_viz_form_submit($form, &$form_state) {
  // Remove unnecessary values.
  form_state_values_clean($form_state);

  // Add the submitted form values to the visualization, and save it.
  $viz = $form['#viz'];
  foreach ($form_state['values'] as $key => $value) {
    $viz->$key = $value;
  }
  $status = clio_analyze_viz_save($viz);
  menu_rebuild();
  
  switch ($status) {
    case SAVED_NEW:
      drupal_set_message(t('Added visualization %viz.', array('%viz' => $viz->label)));
      break;

    case SAVED_UPDATED:
      drupal_set_message(t('The visualization %viz has been updated.', array('%viz' => $viz->label)));
      break;
  }
  drupal_goto('admin/config/clio/analyze');
}

