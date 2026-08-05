module.exports = [
  {
    type: 'heading',
    defaultValue: 'Watchface Settings',
  },
  {
    type: 'section',
    items: [
      {
        type: 'radiogroup',
        messageKey: 'CATPPUCCIN_FLAVOR',
        label: 'Flavor',
        defaultValue: 'mocha',
        options: [
          { label: 'Latte', value: 'latte' },
          { label: 'Frappe', value: 'frappe' },
          { label: 'Macchiato', value: 'macchiato' },
          { label: 'Mocha', value: 'mocha' },
        ],
      },
      {
        type: 'toggle',
        messageKey: 'SHOW_HEARTRATE',
        label: 'Show Heart Rate',
        defaultValue: false,
        description: 'Show heart icon + BPM in the top-left corner'
      },
      {
        type: 'input',
        messageKey: 'SHOW_HEARTRATE_INTERVAL',
        label: 'Heart-rate Interval (s)',
        defaultValue: '60',
        description: 'Seconds between heart-rate readings (min 15).',
        attributes: {
          type: 'number',
          min: 15,
          max: 3600,
          step: 1
        }
      },
    ],
  },
  {
    type: 'submit',
    defaultValue: 'Save Settings',
  },
];
